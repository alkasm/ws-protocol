"""
@generated by mypy-protobuf.  Do not edit manually!
isort:skip_file
"""
import builtins
import foxglove.Color_pb2
import foxglove.Pose_pb2
import foxglove.Vector3_pb2
import google.protobuf.descriptor
import google.protobuf.message
import typing
import typing_extensions

DESCRIPTOR: google.protobuf.descriptor.FileDescriptor

class SpherePrimitive(google.protobuf.message.Message):
    """A primitive representing a sphere or ellipsoid"""
    DESCRIPTOR: google.protobuf.descriptor.Descriptor
    POSE_FIELD_NUMBER: builtins.int
    SIZE_FIELD_NUMBER: builtins.int
    COLOR_FIELD_NUMBER: builtins.int
    @property
    def pose(self) -> foxglove.Pose_pb2.Pose:
        """Position of the center of the sphere and orientation of the sphere"""
        pass
    @property
    def size(self) -> foxglove.Vector3_pb2.Vector3:
        """Size (diameter) of the sphere along each axis"""
        pass
    @property
    def color(self) -> foxglove.Color_pb2.Color:
        """Color of the sphere"""
        pass
    def __init__(self,
        *,
        pose: typing.Optional[foxglove.Pose_pb2.Pose] = ...,
        size: typing.Optional[foxglove.Vector3_pb2.Vector3] = ...,
        color: typing.Optional[foxglove.Color_pb2.Color] = ...,
        ) -> None: ...
    def HasField(self, field_name: typing_extensions.Literal["color",b"color","pose",b"pose","size",b"size"]) -> builtins.bool: ...
    def ClearField(self, field_name: typing_extensions.Literal["color",b"color","pose",b"pose","size",b"size"]) -> None: ...
global___SpherePrimitive = SpherePrimitive
