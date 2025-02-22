"""
@generated by mypy-protobuf.  Do not edit manually!
isort:skip_file
"""
import builtins
import foxglove.Color_pb2
import foxglove.Pose_pb2
import google.protobuf.descriptor
import google.protobuf.message
import typing
import typing_extensions

DESCRIPTOR: google.protobuf.descriptor.FileDescriptor

class ArrowPrimitive(google.protobuf.message.Message):
    """A primitive representing an arrow"""
    DESCRIPTOR: google.protobuf.descriptor.Descriptor
    POSE_FIELD_NUMBER: builtins.int
    SHAFT_LENGTH_FIELD_NUMBER: builtins.int
    SHAFT_DIAMETER_FIELD_NUMBER: builtins.int
    HEAD_LENGTH_FIELD_NUMBER: builtins.int
    HEAD_DIAMETER_FIELD_NUMBER: builtins.int
    COLOR_FIELD_NUMBER: builtins.int
    @property
    def pose(self) -> foxglove.Pose_pb2.Pose:
        """Position of the arrow's tail and orientation of the arrow. Identity orientation means the arrow points in the +x direction."""
        pass
    shaft_length: builtins.float
    """Length of the arrow shaft"""

    shaft_diameter: builtins.float
    """Diameter of the arrow shaft"""

    head_length: builtins.float
    """Length of the arrow head"""

    head_diameter: builtins.float
    """Diameter of the arrow head"""

    @property
    def color(self) -> foxglove.Color_pb2.Color:
        """Color of the arrow"""
        pass
    def __init__(self,
        *,
        pose: typing.Optional[foxglove.Pose_pb2.Pose] = ...,
        shaft_length: builtins.float = ...,
        shaft_diameter: builtins.float = ...,
        head_length: builtins.float = ...,
        head_diameter: builtins.float = ...,
        color: typing.Optional[foxglove.Color_pb2.Color] = ...,
        ) -> None: ...
    def HasField(self, field_name: typing_extensions.Literal["color",b"color","pose",b"pose"]) -> builtins.bool: ...
    def ClearField(self, field_name: typing_extensions.Literal["color",b"color","head_diameter",b"head_diameter","head_length",b"head_length","pose",b"pose","shaft_diameter",b"shaft_diameter","shaft_length",b"shaft_length"]) -> None: ...
global___ArrowPrimitive = ArrowPrimitive
