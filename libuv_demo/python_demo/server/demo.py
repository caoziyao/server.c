# coding: utf-8


def error(code=404):
    """
        根据 code 返回不同的错误响应
        目前只有 404
    """
    e = {
        404: b'HTTP/1.1 404 NOT FOUND\r\n\r\n<h1>NOT FOUND</h1>',
    }
    return e.get(code, b'')


def route_image():
    """
    图片的处理函数, 读取图片并生成响应返回
    """
    with open('doge.gif', 'rb') as f:
        header = b'HTTP/1.1 200 OK\r\nContent-Type: image/gif\r\n'
        img = header + b'\r\n' + f.read()
        return img


def response_for_path(path):
    """
    根据 path 调用相应的处理函数
    没有处理的 path 会返回 404
    """
    r = {
        '/': route_index,
        '/doge.gif': route_image,
    }
    response = r.get(path, error)
    return response()


def route_index():
    """
    主页的处理函数, img
    """
    # <img src="/doge.gif">
    header = b'HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n'
    body = b'<h1>Hello World</h1>'
    r = header + b'\r\n' + body
    return r


def main(request):
    if request:
        path = request.split()[1]
        print('request', path)

        r = response_for_path(path)
        return r


if __name__ == '__main__':
    main()
