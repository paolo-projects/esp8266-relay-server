import io
import json

KEY_TYPE = b'\x10'
VALUE_TYPE = b'\x11'


class SerialMap:
    data: dict

    def __init__(self, data: bytes = None):
        self.data = {}

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

        return result.getvalue()


def test():
    map = SerialMap()

    map.put("the key", "the value")
    print("Dictionary:", json.dumps(map.data))
    print("Serialized:", map.serialize())

    map2 = SerialMap(
        b'\x10\x05hello\x11\x05world\x10\x07another\x11\x05value')
    print("Serialized:", b'\x10\x05hello\x11\x05world\x10\x07another\x11\x05value')
    print("Dictionary:", json.dumps(map2.data))
