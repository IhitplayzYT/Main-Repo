// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from tutorial_interfaces:srv/Display.idl
// generated code does not contain a copyright notice

#ifndef TUTORIAL_INTERFACES__SRV__DETAIL__DISPLAY__TRAITS_HPP_
#define TUTORIAL_INTERFACES__SRV__DETAIL__DISPLAY__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "tutorial_interfaces/srv/detail/display__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace tutorial_interfaces
{

namespace srv
{

inline void to_flow_style_yaml(
  const Display_Request & msg,
  std::ostream & out)
{
  out << "{";
  // member: number
  {
    out << "number: ";
    rosidl_generator_traits::value_to_yaml(msg.number, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const Display_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: number
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "number: ";
    rosidl_generator_traits::value_to_yaml(msg.number, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const Display_Request & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace srv

}  // namespace tutorial_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use tutorial_interfaces::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const tutorial_interfaces::srv::Display_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  tutorial_interfaces::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use tutorial_interfaces::srv::to_yaml() instead")]]
inline std::string to_yaml(const tutorial_interfaces::srv::Display_Request & msg)
{
  return tutorial_interfaces::srv::to_yaml(msg);
}

template<>
inline const char * data_type<tutorial_interfaces::srv::Display_Request>()
{
  return "tutorial_interfaces::srv::Display_Request";
}

template<>
inline const char * name<tutorial_interfaces::srv::Display_Request>()
{
  return "tutorial_interfaces/srv/Display_Request";
}

template<>
struct has_fixed_size<tutorial_interfaces::srv::Display_Request>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<tutorial_interfaces::srv::Display_Request>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<tutorial_interfaces::srv::Display_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace tutorial_interfaces
{

namespace srv
{

inline void to_flow_style_yaml(
  const Display_Response & msg,
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
  const Display_Response & msg,
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

inline std::string to_yaml(const Display_Response & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace srv

}  // namespace tutorial_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use tutorial_interfaces::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const tutorial_interfaces::srv::Display_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  tutorial_interfaces::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use tutorial_interfaces::srv::to_yaml() instead")]]
inline std::string to_yaml(const tutorial_interfaces::srv::Display_Response & msg)
{
  return tutorial_interfaces::srv::to_yaml(msg);
}

template<>
inline const char * data_type<tutorial_interfaces::srv::Display_Response>()
{
  return "tutorial_interfaces::srv::Display_Response";
}

template<>
inline const char * name<tutorial_interfaces::srv::Display_Response>()
{
  return "tutorial_interfaces/srv/Display_Response";
}

template<>
struct has_fixed_size<tutorial_interfaces::srv::Display_Response>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<tutorial_interfaces::srv::Display_Response>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<tutorial_interfaces::srv::Display_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<tutorial_interfaces::srv::Display>()
{
  return "tutorial_interfaces::srv::Display";
}

template<>
inline const char * name<tutorial_interfaces::srv::Display>()
{
  return "tutorial_interfaces/srv/Display";
}

template<>
struct has_fixed_size<tutorial_interfaces::srv::Display>
  : std::integral_constant<
    bool,
    has_fixed_size<tutorial_interfaces::srv::Display_Request>::value &&
    has_fixed_size<tutorial_interfaces::srv::Display_Response>::value
  >
{
};

template<>
struct has_bounded_size<tutorial_interfaces::srv::Display>
  : std::integral_constant<
    bool,
    has_bounded_size<tutorial_interfaces::srv::Display_Request>::value &&
    has_bounded_size<tutorial_interfaces::srv::Display_Response>::value
  >
{
};

template<>
struct is_service<tutorial_interfaces::srv::Display>
  : std::true_type
{
};

template<>
struct is_service_request<tutorial_interfaces::srv::Display_Request>
  : std::true_type
{
};

template<>
struct is_service_response<tutorial_interfaces::srv::Display_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // TUTORIAL_INTERFACES__SRV__DETAIL__DISPLAY__TRAITS_HPP_
