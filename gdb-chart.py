import socket


class ChartLine(gdb.Function):
    """Sends a value or an array of values to chart app network socket
    """

    def __init__(self):
        super(ChartLine, self).__init__("chartline")

    def invoke(self, var):
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        while var.type.strip_typedefs().code == gdb.TYPE_CODE_PTR:
            var = var.referenced_value()
        try:
            s.connect(("127.0.0.1", 19022))
            s.sendall(b'**\n')
            s.sendall(self._printDataByValue(var))
        except Exception as e:
            print e
            raise e
        finally:
            s.close()
            # gdb.execute("c")
        return gdb.Value(True)

    def _printDataByValue(self, value):
        if (value.type.strip_typedefs().code == gdb.TYPE_CODE_ARRAY):
            value.fetch_lazy()
            arrayRange = value.type.range()
            s = '\n'.join(map(lambda i: str(value[i]), range(arrayRange[0], arrayRange[1] + 1)))
            return s
        else:
            return str(value) + '\n'


ChartLine()
