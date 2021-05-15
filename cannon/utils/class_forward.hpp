#pragma once
#ifndef CANNON_UTILS_CLASS_FORWARD_H
#define CANNON_UTILS_CLASS_FORWARD_H 

/*!
 * \file cannon/utils/class_forward.hpp
 * \brief File containing class forward macro.
 */

#include <memory>

/*!
 * \def CANNON_CLASS_FORWARD
 * \brief Macro that forward declares a class, <Class>Ptr, <Class>ConstPtr.
 */
#define CANNON_CLASS_FORWARD(C)                 \
  class C;                                      \
  typedef std::shared_ptr<C> C##Ptr;            \
  typedef std::shared_ptr<const C> C##ConstPrt; 

#endif /* ifndef CANNON_UTILS_CLASS_FORWARD_H */
