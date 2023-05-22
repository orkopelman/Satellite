#ifndef FSL_ENDIANS_HPP
#define FSL_ENDIANS_HPP

#include <cstddef>
#include <cstdint>

namespace fsl {

template<typename T>
T TranslateToNetEndian(T a_value);

template<typename T>
T TranslateToHostEndian(T a_value);

template<typename T>
void ArrayToNetEndian(void* a_array, size_t a_arraySize);

template<typename T>
void ArrayToHostEndian(void* a_array, size_t a_arraySize);

bool IsNetEndian() noexcept;

} // fsl

#include "inl/fsl_endians.hxx"

#endif /* FSL_ENDIANS_HPP */