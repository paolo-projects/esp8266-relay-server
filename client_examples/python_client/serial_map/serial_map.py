import io
import json
from typing import Dict

KEY_TYPE = b'\x10'
VALUE_TYPE = b'\x11'


class SerialMap:
    data: Dict[str, str]

    def __init__(self, data: bytes = None, values: Dict[str, str] = None):
        self.data = values or {}

        if data is not None:
            buff = io.BytesIO(data)

            while buff.readable():
                if buff.read(1) != KEY_TYPE:
                    return
                len = int.from_bytes(buff.read(1), "big", signed=False)
                key = buff.read(len).decode("utf8")

                if buff.read(1) != VALUE_TYPE:
                    return
                len = int.from_bytes(buff.read(1), "big", signed=False)
                value = buff.read(len).decode("utf8")

                self.data[key] = value

    def put(self, key: str, value: str):
        self.data[key] = value

    def has(self, key: str) -> bool:
        return key in self.data

    def get(self, key: str) -> str:
        return self.data[key]

    def serialize(self) -> bytes:
        result = io.BytesIO()

        for key, value in self.data.items():
            result.write(KEY_TYPE)
            result.write(len(key).to_bytes(1, "big", signed=False))
            result.write(bytes(key, "utf8"))

            result.write(VALUE_TYPE)
            result.write(len(value).to_bytes(1, "big", signed=False))
            result.write(bytes(value, "utf8"))

        result.write(b'\0')

        return result.getvalue()

    def __getitem__(self, key: str) -> str:
        return self.data[key]

    def __setitem__(self, key: str, value: str) -> str:
        self.data[key] = value

    def __delitem__(self, key: str):
        del self.data[key]

    def __contains__(self, key: str):
        return key in self.data

    def __str__(self) -> str:
        return json.dumps(self.data)

    def __bytes__(self) -> bytes:
        return self.serialize()
