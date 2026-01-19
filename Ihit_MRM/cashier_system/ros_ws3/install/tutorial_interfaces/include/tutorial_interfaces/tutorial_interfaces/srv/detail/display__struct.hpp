// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from tutorial_interfaces:srv/Display.idl
// generated code does not contain a copyright notice

#ifndef TUTORIAL_INTERFACES__SRV__DETAIL__DISPLAY__STRUCT_HPP_
#define TUTORIAL_INTERFACES__SRV__DETAIL__DISPLAY__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__tutorial_interfaces__srv__Display_Request __attribute__((deprecated))
#else
# define DEPRECATED__tutorial_interfaces__srv__Display_Request __declspec(deprecated)
#endif

namespace tutorial_interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct Display_Request_
{
  using Type = Display_Request_<ContainerAllocator>;

  explicit Display_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->number = 0l;
    }
  }

  explicit Display_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->number = 0l;
    }
  }

  // field types and members
  using _number_type =
    int32_t;
  _number_type number;

  // setters for named parameter idiom
  Type & set__number(
    const int32_t & _arg)
  {
    this->number = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    tutorial_interfaces::srv::Display_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const tutorial_interfaces::srv::Display_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<tutorial_interfaces::srv::Display_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<tutorial_interfaces::srv::Display_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      tutorial_interfaces::srv::Display_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<tutorial_interfaces::srv::Display_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      tutorial_interfaces::srv::Display_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<tutorial_interfaces::srv::Display_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<tutorial_interfaces::srv::Display_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<tutorial_interfaces::srv::Display_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__tutorial_interfaces__srv__Display_Request
    std::shared_ptr<tutorial_interfaces::srv::Display_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__tutorial_interfaces__srv__Display_Request
    std::shared_ptr<tutorial_interfaces::srv::Display_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const Display_Request_ & other) const
  {
    if (this->number != other.number) {
      return false;
    }
    return true;
  }
  bool operator!=(const Display_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct Display_Request_

// alias to use template instance with default allocator
using Display_Request =
  tutorial_interfaces::srv::Display_Request_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace tutorial_interfaces


#ifndef _WIN32
# define DEPRECATED__tutorial_interfaces__srv__Display_Response __attribute__((deprecated))
#else
# define DEPRECATED__tutorial_interfaces__srv__Display_Response __declspec(deprecated)
#endif

namespace tutorial_interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct Display_Response_
{
  using Type = Display_Response_<ContainerAllocator>;

  explicit Display_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->bill_no = 0l;
      this->time = "";
      this->unit_type = 0;
      this->unit_no = 0l;
      this->unit_price = 0.0f;
      this->total = 0.0f;
    }
  }

  explicit Display_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : time(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->bill_no = 0l;
      this->time = "";
      this->unit_type = 0;
      this->unit_no = 0l;
      this->unit_price = 0.0f;
      this->total = 0.0f;
    }
  }

  // field types and members
  using _bill_no_type =
    int32_t;
  _bill_no_type bill_no;
  using _time_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _time_type time;
  using _unit_type_type =
    uint8_t;
  _unit_type_type unit_type;
  using _unit_no_type =
    int32_t;
  _unit_no_type unit_no;
  using _unit_price_type =
    float;
  _unit_price_type unit_price;
  using _total_type =
    float;
  _total_type total;

  // setters for named parameter idiom
  Type & set__bill_no(
    const int32_t & _arg)
  {
    this->bill_no = _arg;
    return *this;
  }
  Type & set__time(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->time = _arg;
    return *this;
  }
  Type & set__unit_type(
    const uint8_t & _arg)
  {
    this->unit_type = _arg;
    return *this;
  }
  Type & set__unit_no(
    const int32_t & _arg)
  {
    this->unit_no = _arg;
    return *this;
  }
  Type & set__unit_price(
    const float & _arg)
  {
    this->unit_price = _arg;
    return *this;
  }
  Type & set__total(
    const float & _arg)
  {
    this->total = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    tutorial_interfaces::srv::Display_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const tutorial_interfaces::srv::Display_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<tutorial_interfaces::srv::Display_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<tutorial_interfaces::srv::Display_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      tutorial_interfaces::srv::Display_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<tutorial_interfaces::srv::Display_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      tutorial_interfaces::srv::Display_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<tutorial_interfaces::srv::Display_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<tutorial_interfaces::srv::Display_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<tutorial_interfaces::srv::Display_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__tutorial_interfaces__srv__Display_Response
    std::shared_ptr<tutorial_interfaces::srv::Display_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__tutorial_interfaces__srv__Display_Response
    std::shared_ptr<tutorial_interfaces::srv::Display_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const Display_Response_ & other) const
  {
    if (this->bill_no != other.bill_no) {
      return false;
    }
    if (this->time != other.time) {
      return false;
    }
    if (this->unit_type != other.unit_type) {
      return false;
    }
    if (this->unit_no != other.unit_no) {
      return false;
    }
    if (this->unit_price != other.unit_price) {
      return false;
    }
    if (this->total != other.total) {
      return false;
    }
    return true;
  }
  bool operator!=(const Display_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct Display_Response_

// alias to use template instance with default allocator
using Display_Response =
  tutorial_interfaces::srv::Display_Response_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace tutorial_interfaces

namespace tutorial_interfaces
{

namespace srv
{

struct Display
{
  using Request = tutorial_interfaces::srv::Display_Request;
  using Response = tutorial_interfaces::srv::Display_Response;
};

}  // namespace srv

}  // namespace tutorial_interfaces

#endif  // TUTORIAL_INTERFACES__SRV__DETAIL__DISPLAY__STRUCT_HPP_
