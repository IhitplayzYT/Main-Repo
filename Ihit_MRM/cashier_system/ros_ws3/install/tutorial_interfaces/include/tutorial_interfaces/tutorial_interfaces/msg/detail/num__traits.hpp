// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from tutorial_interfaces:msg/Num.idl
// generated code does not contain a copyright notice

#ifndef TUTORIAL_INTERFACES__MSG__DETAIL__NUM__TRAITS_HPP_
#define TUTORIAL_INTERFACES__MSG__DETAIL__NUM__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "tutorial_interfaces/msg/detail/num__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace tutorial_interfaces
{

namespace msg
{

inline void to_flow_style_yaml(
  const Num & msg,
  std::ostream & out)
{
  out << "{";
  // member: bill_no
  {
    out << "bill_no: ";
    rosidl_generator_traits::value_to_yaml(msg.bill_no, out);
    out << ", ";
  }

  // member: time
  {
    out << "time: ";
    rosidl_generator_traits::value_to_yaml(msg.time, out);
    out << ", ";
  }

  // member: unit_type
  {
    out << "unit_type: ";
    rosidl_generator_traits::value_to_yaml(msg.unit_type, out);
    out << ", ";
  }

  // member: unit_no
  {
    out << "unit_no: ";
    rosidl_generator_traits::value_to_yaml(msg.unit_no, out);
    out << ", ";
  }

  // member: unit_price
  {
    out << "unit_price: ";
    rosidl_generator_traits::value_to_yaml(msg.unit_price, out);
    out << ", ";
  }

  // member: total
  {
    out << "total: ";
    rosidl_generator_traits::value_to_yaml(msg.total, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const Num & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: bill_no
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "bill_no: ";
    rosidl_generator_traits::value_to_yaml(msg.bill_no, out);
    out << "\n";
  }

  // member: time
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "time: ";
    rosidl_generator_traits::value_to_yaml(msg.time, out);
    out << "\n";
  }

  // member: unit_type
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "unit_type: ";
    rosidl_generator_traits::value_to_yaml(msg.unit_type, out);
    out << "\n";
  }

  // member: unit_no
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "unit_no: ";
    rosidl_generator_traits::value_to_yaml(msg.unit_no, out);
    out << "\n";
  }

  // member: unit_price
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "unit_price: ";
    rosidl_generator_traits::value_to_yaml(msg.unit_price, out);
    out << "\n";
  }

  // member: total
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "total: ";
    rosidl_generator_traits::value_to_yaml(msg.total, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const Num & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace msg

}  // namespace tutorial_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use tutorial_interfaces::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const tutorial_interfaces::msg::Num & msg,
  std::ostream & out, size_t indentation = 0)
{
  tutorial_interfaces::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use tutorial_interfaces::msg::to_yaml() instead")]]
inline std::string to_yaml(const tutorial_interfaces::msg::Num & msg)
{
  return tutorial_interfaces::msg::to_yaml(msg);
}

template<>
inline const char * data_type<tutorial_interfaces::msg::Num>()
{
  return "tutorial_interfaces::msg::Num";
}

template<>
inline const char * name<tutorial_interfaces::msg::Num>()
{
  return "tutorial_interfaces/msg/Num";
}

template<>
struct has_fixed_size<tutorial_interfaces::msg::Num>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<tutorial_interfaces::msg::Num>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<tutorial_interfaces::msg::Num>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // TUTORIAL_INTERFACES__MSG__DETAIL__NUM__TRAITS_HPP_
