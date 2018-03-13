
string.split = function(s, p)
    local rt= {}
    string.gsub(s, '[^'..p..']+', function(w) table.insert(rt, w) end )
    return rt
end

route_index = function ()
    local header = 'HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n'
    local body = '<h1>Hello World aas</h1>'
    local r = header..body
    return r
end

route_error = function ()
    local header = 'HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n'
    local body = '<h1>404</h1>'
    local r = header..body
    return r
end

luaadd = function(request)
    local str = request

    local list = string.split(str, ' ')
    local path = list[2]

    local r = ''
    if path == '/' then
        r = route_index()
    else
        r = route_error()
    end

    return r
end
