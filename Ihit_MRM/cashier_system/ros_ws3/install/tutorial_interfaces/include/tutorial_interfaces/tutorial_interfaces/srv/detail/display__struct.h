// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from tutorial_interfaces:srv/Display.idl
// generated code does not contain a copyright notice

#ifndef TUTORIAL_INTERFACES__SRV__DETAIL__DISPLAY__STRUCT_H_
#define TUTORIAL_INTERFACES__SRV__DETAIL__DISPLAY__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in srv/Display in the package tutorial_interfaces.
typedef struct tutorial_interfaces__srv__Display_Request
{
  int32_t number;
} tutorial_interfaces__srv__Display_Request;

// Struct for a sequence of tutorial_interfaces__srv__Display_Request.
typedef struct tutorial_interfaces__srv__Display_Request__Sequence
{
  tutorial_interfaces__srv__Display_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} tutorial_interfaces__srv__Display_Request__Sequence;


// Constants defined in the message

// Include directives for member types
// Member 'time'
#include "rosidl_runtime_c/string.h"

/// Struct defined in srv/Display in the package tutorial_interfaces.
typedef struct tutorial_interfaces__srv__Display_Response
{
  int32_t bill_no;
  rosidl_runtime_c__String time;
  uint8_t unit_type;
  int32_t unit_no;
  float unit_price;
  float total;
} tutorial_interfaces__srv__Display_Response;

// Struct for a sequence of tutorial_interfaces__srv__Display_Response.
typedef struct tutorial_interfaces__srv__Display_Response__Sequence
{
  tutorial_interfaces__srv__Display_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} tutorial_interfaces__srv__Display_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // TUTORIAL_INTERFACES__SRV__DETAIL__DISPLAY__STRUCT_H_
