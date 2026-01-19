# generated from rosidl_generator_py/resource/_idl.py.em
# with input from tutorial_interfaces:srv/Display.idl
# generated code does not contain a copyright notice


# Import statements for member types

import builtins  # noqa: E402, I100

import rosidl_parser.definition  # noqa: E402, I100


class Metaclass_Display_Request(type):
    """Metaclass of message 'Display_Request'."""

    _CREATE_ROS_MESSAGE = None
    _CONVERT_FROM_PY = None
    _CONVERT_TO_PY = None
    _DESTROY_ROS_MESSAGE = None
    _TYPE_SUPPORT = None

    __constants = {
    }

    @classmethod
    def __import_type_support__(cls):
        try:
            from rosidl_generator_py import import_type_support
            module = import_type_support('tutorial_interfaces')
        except ImportError:
            import logging
            import traceback
            logger = logging.getLogger(
                'tutorial_interfaces.srv.Display_Request')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__srv__display__request
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__srv__display__request
            cls._CONVERT_TO_PY = module.convert_to_py_msg__srv__display__request
            cls._TYPE_SUPPORT = module.type_support_msg__srv__display__request
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__srv__display__request

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
        }


class Display_Request(metaclass=Metaclass_Display_Request):
    """Message class 'Display_Request'."""

    __slots__ = [
        '_number',
    ]

    _fields_and_field_types = {
        'number': 'int32',
    }

    SLOT_TYPES = (
        rosidl_parser.definition.BasicType('int32'),  # noqa: E501
    )

    def __init__(self, **kwargs):
        assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
            'Invalid arguments passed to constructor: %s' % \
            ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        self.number = kwargs.get('number', int())

    def __repr__(self):
        typename = self.__class__.__module__.split('.')
        typename.pop()
        typename.append(self.__class__.__name__)
        args = []
        for s, t in zip(self.__slots__, self.SLOT_TYPES):
            field = getattr(self, s)
            fieldstr = repr(field)
            # We use Python array type for fields that can be directly stored
            # in them, and "normal" sequences for everything else.  If it is
            # a type that we store in an array, strip off the 'array' portion.
            if (
                isinstance(t, rosidl_parser.definition.AbstractSequence) and
                isinstance(t.value_type, rosidl_parser.definition.BasicType) and
                t.value_type.typename in ['float', 'double', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'int64', 'uint64']
            ):
                if len(field) == 0:
                    fieldstr = '[]'
                else:
                    assert fieldstr.startswith('array(')
                    prefix = "array('X', "
                    suffix = ')'
                    fieldstr = fieldstr[len(prefix):-len(suffix)]
            args.append(s[1:] + '=' + fieldstr)
        return '%s(%s)' % ('.'.join(typename), ', '.join(args))

    def __eq__(self, other):
        if not isinstance(other, self.__class__):
            return False
        if self.number != other.number:
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)

    @builtins.property
    def number(self):
        """Message field 'number'."""
        return self._number

    @number.setter
    def number(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'number' field must be of type 'int'"
            assert value >= -2147483648 and value < 2147483648, \
                "The 'number' field must be an integer in [-2147483648, 2147483647]"
        self._number = value


# Import statements for member types

# already imported above
# import builtins

import math  # noqa: E402, I100

# already imported above
# import rosidl_parser.definition


class Metaclass_Display_Response(type):
    """Metaclass of message 'Display_Response'."""

    _CREATE_ROS_MESSAGE = None
    _CONVERT_FROM_PY = None
    _CONVERT_TO_PY = None
    _DESTROY_ROS_MESSAGE = None
    _TYPE_SUPPORT = None

    __constants = {
    }

    @classmethod
    def __import_type_support__(cls):
        try:
            from rosidl_generator_py import import_type_support
            module = import_type_support('tutorial_interfaces')
        except ImportError:
            import logging
            import traceback
            logger = logging.getLogger(
                'tutorial_interfaces.srv.Display_Response')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__srv__display__response
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__srv__display__response
            cls._CONVERT_TO_PY = module.convert_to_py_msg__srv__display__response
            cls._TYPE_SUPPORT = module.type_support_msg__srv__display__response
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__srv__display__response

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
        }


class Display_Response(metaclass=Metaclass_Display_Response):
    """Message class 'Display_Response'."""

    __slots__ = [
        '_bill_no',
        '_time',
        '_unit_type',
        '_unit_no',
        '_unit_price',
        '_total',
    ]

    _fields_and_field_types = {
        'bill_no': 'int32',
        'time': 'string',
        'unit_type': 'uint8',
        'unit_no': 'int32',
        'unit_price': 'float',
        'total': 'float',
    }

    SLOT_TYPES = (
        rosidl_parser.definition.BasicType('int32'),  # noqa: E501
        rosidl_parser.definition.UnboundedString(),  # noqa: E501
        rosidl_parser.definition.BasicType('uint8'),  # noqa: E501
        rosidl_parser.definition.BasicType('int32'),  # noqa: E501
        rosidl_parser.definition.BasicType('float'),  # noqa: E501
        rosidl_parser.definition.BasicType('float'),  # noqa: E501
    )

    def __init__(self, **kwargs):
        assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
            'Invalid arguments passed to constructor: %s' % \
            ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        self.bill_no = kwargs.get('bill_no', int())
        self.time = kwargs.get('time', str())
        self.unit_type = kwargs.get('unit_type', int())
        self.unit_no = kwargs.get('unit_no', int())
        self.unit_price = kwargs.get('unit_price', float())
        self.total = kwargs.get('total', float())

    def __repr__(self):
        typename = self.__class__.__module__.split('.')
        typename.pop()
        typename.append(self.__class__.__name__)
        args = []
        for s, t in zip(self.__slots__, self.SLOT_TYPES):
            field = getattr(self, s)
            fieldstr = repr(field)
            # We use Python array type for fields that can be directly stored
            # in them, and "normal" sequences for everything else.  If it is
            # a type that we store in an array, strip off the 'array' portion.
            if (
                isinstance(t, rosidl_parser.definition.AbstractSequence) and
                isinstance(t.value_type, rosidl_parser.definition.BasicType) and
                t.value_type.typename in ['float', 'double', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'int64', 'uint64']
            ):
                if len(field) == 0:
                    fieldstr = '[]'
                else:
                    assert fieldstr.startswith('array(')
                    prefix = "array('X', "
                    suffix = ')'
                    fieldstr = fieldstr[len(prefix):-len(suffix)]
            args.append(s[1:] + '=' + fieldstr)
        return '%s(%s)' % ('.'.join(typename), ', '.join(args))

    def __eq__(self, other):
        if not isinstance(other, self.__class__):
            return False
        if self.bill_no != other.bill_no:
            return False
        if self.time != other.time:
            return False
        if self.unit_type != other.unit_type:
            return False
        if self.unit_no != other.unit_no:
            return False
        if self.unit_price != other.unit_price:
            return False
        if self.total != other.total:
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)

    @builtins.property
    def bill_no(self):
        """Message field 'bill_no'."""
        return self._bill_no

    @bill_no.setter
    def bill_no(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'bill_no' field must be of type 'int'"
            assert value >= -2147483648 and value < 2147483648, \
                "The 'bill_no' field must be an integer in [-2147483648, 2147483647]"
        self._bill_no = value

    @builtins.property
    def time(self):
        """Message field 'time'."""
        return self._time

    @time.setter
    def time(self, value):
        if __debug__:
            assert \
                isinstance(value, str), \
                "The 'time' field must be of type 'str'"
        self._time = value

    @builtins.property
    def unit_type(self):
        """Message field 'unit_type'."""
        return self._unit_type

    @unit_type.setter
    def unit_type(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'unit_type' field must be of type 'int'"
            assert value >= 0 and value < 256, \
                "The 'unit_type' field must be an unsigned integer in [0, 255]"
        self._unit_type = value

    @builtins.property
    def unit_no(self):
        """Message field 'unit_no'."""
        return self._unit_no

    @unit_no.setter
    def unit_no(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'unit_no' field must be of type 'int'"
            assert value >= -2147483648 and value < 2147483648, \
                "The 'unit_no' field must be an integer in [-2147483648, 2147483647]"
        self._unit_no = value

    @builtins.property
    def unit_price(self):
        """Message field 'unit_price'."""
        return self._unit_price

    @unit_price.setter
    def unit_price(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'unit_price' field must be of type 'float'"
            assert not (value < -3.402823466e+38 or value > 3.402823466e+38) or math.isinf(value), \
                "The 'unit_price' field must be a float in [-3.402823466e+38, 3.402823466e+38]"
        self._unit_price = value

    @builtins.property
    def total(self):
        """Message field 'total'."""
        return self._total

    @total.setter
    def total(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'total' field must be of type 'float'"
            assert not (value < -3.402823466e+38 or value > 3.402823466e+38) or math.isinf(value), \
                "The 'total' field must be a float in [-3.402823466e+38, 3.402823466e+38]"
        self._total = value


class Metaclass_Display(type):
    """Metaclass of service 'Display'."""

    _TYPE_SUPPORT = None

    @classmethod
    def __import_type_support__(cls):
        try:
            from rosidl_generator_py import import_type_support
            module = import_type_support('tutorial_interfaces')
        except ImportError:
            import logging
            import traceback
            logger = logging.getLogger(
                'tutorial_interfaces.srv.Display')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._TYPE_SUPPORT = module.type_support_srv__srv__display

            from tutorial_interfaces.srv import _display
            if _display.Metaclass_Display_Request._TYPE_SUPPORT is None:
                _display.Metaclass_Display_Request.__import_type_support__()
            if _display.Metaclass_Display_Response._TYPE_SUPPORT is None:
                _display.Metaclass_Display_Response.__import_type_support__()


class Display(metaclass=Metaclass_Display):
    from tutorial_interfaces.srv._display import Display_Request as Request
    from tutorial_interfaces.srv._display import Display_Response as Response

    def __init__(self):
        raise NotImplementedError('Service classes can not be instantiated')
