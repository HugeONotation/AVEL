# Fixed-point Vectors

## Vector Classes:

### Type Aliases
`using scaler = ...;`
* the type of teh vector's elements

`using primitive = ...;`
* backing type

`using mask = Vector_mask<scalar, width>;`
* corresponding instantiation of `avel::Vector_mask`

`using rebind_type<U>`
* corresponding instantiation of `avel::Vector<U, width>`

`using rebind_width<M>`
* corresponding instantiation of `avel::Vector<scalar, M>`

### Constructors
`Vector(Vector<U, N> v)` where `U` is convertible to `T`
* for each lane, converts `v` to be of type `T` in manner consistent with 
  `avel::convert` function

`Vector(mask m)`
* for each lane, produces `1` if `m` is set and `0` otherwise

`explicit Vector(scalar x)`
* set each lane to be a copy of `x`

`explicit Vector(primitive p)`
* assigns internal variable to be `p`

Defaulted default/copy/move constructors

`Vector(const arrNxT arr)`
* loads elements from `arr` into lanes of vector

### Assignment Operators
`Vector& operator=(primitive p)`
* assigns internal variable to `p`

`vector& operator=(scalar x)`
* set each lane to be a copy of `x`

### Comparison Operators
`mask operator==(Vector lhs, Vector rhs) const`
* for each lane, performed an equality comparison between `lhs` and `rhs`

`mask operator!=(Vector lhs, Vector rhs) const`
* for each lane, performed an inequality comparison between `lhs` and `rhs`

`mask operator<(Vector lhs, Vector rhs) const`
* for each lane, performed a less-than comparison between `lhs` and `rhs`

`mask operator<=(Vector lhs, Vector rhs) const`
* for each lane, performed a less-than-or-equal comparison between `lhs` and
  `rhs`

`mask operator>(Vector lhs, Vector rhs) const`
* for each lane, performed a greater-than comparison between `lhs` and `rhs`

`mask operator>=(Vector lhs, Vector rhs) const`
* for each lane, performed a greater-than-or-equal comparison between `lhs`
  and `rhs`

### Arithmetic Assignment Operators
`Vector& operator+=(Vector rhs)`
* for each lane, adds `rhs` to the current value in `this`
* addition is saturated

`Vector& operator-=(Vector rhs)`
* for each lane, subtracts `rhs` from the current value in `this`

`Vector& operator*=(Vector rhs)`
* for each lane, multiplies `rhs` against the current value in `this`

`Vector& operator/=(Vector rhs)`
* for each lane, divides the current value in `this` by `rhs`
* consider use of denominator classes
* output is unspecified for a particular lane if the denominator is 0

`Vector& operator%=(Vector rhs)`
* for each lane, computes the remainder of the dividing `this` by `rhs`
* consider use of denominator classes
* output is unspecified for a particular lane if the denominator is 0

### Arithmetic Operators
`Vector operator+(Vector rhs) const`
* for each lane, produces the sum of `this` and rhs

`Vector operator-(Vector rhs) const`
* for each lane, produces the difference of `this` and rhs

`Vector operator*(Vector rhs) const`
* for each lane, produces the product of `this` and rhs

`Vector operator/(Vector rhs) const`
* for each lane, produces the quotient of `this` and rhs
* output is unspecified for a particular lane if the denominator is 0

`Vector operator%(Vector rhs) const`
* for each lane, produces the modulus of `this` and rhs
* output is unspecified for a particular lane if the denominator is 0

### Arithmetic Operators

### Increment/Decrement Operators

### Conversion Operators

### unary Arithmetic Operators

## Arrangement Operations

## General Vector Operations
* ``

### Fixed-point Specific Operations
* `Fixed_point round(Fixed_point arg)`
* `Fixed_point floor(Fixed_point arg)`
* `Fixed_point ceil(Fixed_point arg)`
* `Fixed_point nearest_int(Fixed_point arg)`
