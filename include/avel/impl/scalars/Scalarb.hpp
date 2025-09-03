#ifndef AVEL_SCALARB_HPP
#define AVEL_SCALARB_HPP

namespace avel {

    AVEL_FINL bool keep(bool m, bool x) {
        return m & x;
    }

    AVEL_FINL bool clear(bool m, bool x) {
        return !m & x;
    }

    AVEL_FINL bool blend(bool m, bool a, bool b) {
        return (m & a) | (!m & b);
    }

}

#endif //AVEL_SCALARB_HPP
