#include <cstdlib>

#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <array>
#include <cmath>
#include <iostream>

#define __USE_FILE_OFFSET64

//#include <avel/Misc.hpp>
#include <bit>

#include <sys/mman.h>
#include <linux/mman.h>
#include <fcntl.h>
#include <string.h>

#include <boost/multiprecision/cpp_bin_float.hpp>

namespace mp = boost::multiprecision;

using hp_float = mp::number<mp::backends::cpp_bin_float<256, mp::backends::digit_base_2, void, std::int16_t, -1024, +1024>, mp::et_off>;

//=========================================================
// Configuration
//=========================================================

const std::string output_path = "/mnt/sda2/Technology/Baselines/f32/remainder_pi_over_4.bin";
const std::uint64_t range_start = 0x00000000ull;
const std::uint64_t range_end   = 0x7f800000ull;
const std::uint64_t file_size = (range_end - range_start) * sizeof(float);
const std::uint64_t thread_count = 8;

const std::uint64_t task_size = 1ull * 256 * 1024;

hp_float pi_over_4 = mp::atan(hp_float{1.0});

hp_float f(hp_float x) {
    return mp::remainder(x, pi_over_4);
}

//=========================================================
// Global State
//=========================================================

std::uint32_t* output_array = nullptr;

std::uint64_t total_tasks = 0;
std::vector<std::array<std::uint64_t, 2>> work_queue;

std::mutex work_queue_mutex;

//=========================================================
// Misc.
//=========================================================

void create_tasks(
    std::uint64_t range_start,
    std::uint64_t range_end,
    std::uint64_t task_size
) {
    std::uint64_t range_size = range_end - range_start;
    std::uint64_t whole_task_count = (range_size / task_size);
    std::uint64_t task_count = whole_task_count + bool(range_size % task_size);

    std::vector<std::array<std::uint64_t, 2>> tasks;
    tasks.resize(task_count);

    // Handle whole tasks
    for (std::uint64_t i = 0; i < whole_task_count; ++i) {
        tasks[i][0] = range_start + (i * task_size);
        tasks[i][1] = range_start + (i * task_size) + task_size;
    }

    // Handle partial tasks
    if (bool(range_size % task_size)) {
        tasks[whole_task_count][0] = task_size * whole_task_count;
        tasks[whole_task_count][1] = range_end;
    }

    total_tasks = task_count;

    work_queue_mutex.lock();
    work_queue.insert(work_queue.end(), tasks.begin(), tasks.end());
    work_queue_mutex.unlock();
}

void evaluator(std::uint32_t id, std::uint64_t begin, std::uint64_t end);

void thread_worker(std::uint32_t id) {
    std::string output_buffer;
    output_buffer.reserve(256);

    output_buffer.append("Thread ");
    output_buffer.append(std::to_string(id));
    output_buffer.append(" launched.\n");

    std::cout << output_buffer;
    output_buffer.resize(0);
    std::cout.flush();

    while (true) {
        std::array<std::uint64_t, 2> task;

        std::uint64_t tasks_remaining = 0;

        // Attempt to retrieve work from queue
        {
            work_queue_mutex.lock();

            if (work_queue.empty()) {
                work_queue_mutex.unlock();
                return;
            }

            task = work_queue.back();
            work_queue.pop_back();
            tasks_remaining = work_queue.size();

            work_queue_mutex.unlock();
        }

        // Report progress
        output_buffer.append(std::to_string(double(tasks_remaining) / double(total_tasks)));
        output_buffer.append(" of tasks remain to be scheduled.\n");
        std::cout << output_buffer;
        output_buffer.resize(0);
        std::cout.flush();

        // Perform work
        evaluator(id, task[0], task[1]);
    }
}

void evaluator(std::uint32_t id, std::uint64_t begin, std::uint64_t end) {
    //std::stringstream initial_report;
    //initial_report << "Thread " << id << " working on 0x" << std::hex << begin << " to 0x" << std::hex << end << std::endl;
    //std::cout << initial_report.str();

    //auto report_threshold = (end - begin) / 100;

    for (std::uint64_t i = begin; i < end; ++i) {
        std::uint32_t x_bits = static_cast<std::uint32_t>(i);
        float lp_x = std::bit_cast<float>(x_bits);

        hp_float x{lp_x};
        hp_float f_of_x = f(x);
        float lp_f_of_x = static_cast<float>(f_of_x);
        std::uint32_t f_of_x_bits = std::bit_cast<std::uint32_t>(lp_f_of_x);

        output_array[i - range_start] = f_of_x_bits;

        //if ((i - begin) % report_threshold == 0) {
        //    std::string output = "Thread ";
        //    output += std::to_string(id);
        //    output += " at ";
        //    output += std::to_string(double(i - begin) / double(end - begin));
        //    output += "\n";
        //    std::cout << output;
        //    std::cout.flush();
        //}
    }

    //std::string output = "Thread ";
    //output += std::to_string(id);
    //output += " completed\n";
    //std::cout << output;
    //std::cout.flush();
}

void generate_baselines() {
    // Acquire file descriptor
    int fd = open(output_path.c_str(), O_CREAT | O_RDWR, S_IRWXU | S_IWUSR);
    if (fd == -1) {
        std::cerr << "Failed to open file descriptor. Error: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    // Resize file to appropriate capacity
    if (ftruncate(fd, file_size)) {
        std::cerr << "Failed to resize file. Error: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    // Map file
    output_array = reinterpret_cast<std::uint32_t*>(
        mmap(
            nullptr,
            file_size,
            PROT_WRITE,
            MAP_SHARED | MAP_HUGE_1GB,
            fd,
            sizeof(std::uint32_t) * 0x00 //Mapping offset
        )
    );

    if (output_array == MAP_FAILED) {
        std::cerr << "Failed to map file contents. Error: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    // Populate work queue
    create_tasks(range_start, range_end, task_size);

    // Dispatch threads
    std::vector<std::thread> threads;
    for (std::uint32_t i = 0; i < thread_count; ++i) {
        threads.emplace_back(thread_worker, i);
    }

    // Wait on threads
    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Flushing writes to disk" << std::endl;
    msync(output_array, file_size, MS_SYNC);
    munmap(output_array, file_size);
    close(fd);
}

int main() {
    generate_baselines();

    return EXIT_SUCCESS;
}
