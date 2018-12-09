HTTP Test App with ESP32
============================

This is a simple app which demonstrates how to connect to a Wi-Fi network
and then issue HTTP GET and HTTP POST requests.

Setup
------

Set `SSID` and `PASSWORD` to valid settings for a Wi-Fi network in range of the
ESP32 device.

Expected Output
---------------

```
[HTTP Example]
Example app to make HTTP requests.
Ensuring the ESP32 is in station mode...done
Connecting to the Wi-Fi network.
Successfully connected to Wi-Fi network
Querying our IP address...done
IP address: 192.168.86.223
Making HTTP GET request
response:
HTTP/1.1 200 OK
Content-Type: application/json; charset=utf-8
Date: Mon, 10 Dec 2018 00:12:22 GMT
ETag: W/"b9-MyG3MA3JZp1Op6QaGiNIgAew4TY"
Server: nginx
set-cookie: sails.sid=s%3AdXFg6W4nF4KugF4jpxPesCHOaucUK8nI.yZA9M1GYt98p6TGBw69h0u9wJSaGkkg43D6edzZAIwI; Path=/; HttpOnly
Vary: Accept-Encoding
Content-Length: 185
Connection: keep-alive

{"args":{"foo1":"bar1","foo2":"bar2"},"headers":{"x-forwarded-proto":"https","host":"postman-echo.com","x-forwarded-port":"80"},"url":"https://postman-echo.com/get?foo1=bar1&foo2=bar2"}

Making HTTP POST request
response:
HTTP/1.1 200 OK
Content-Type: application/json; charset=utf-8
Date: Mon, 10 Dec 2018 00:12:22 GMT
ETag: W/"110-+4uQxI3jWZWGUykM23yeBMiqmnc"
Server: nginx
set-cookie: sails.sid=s%3A6T_Io5j4xxi3KYh5oQ2M1STZzRl2rG77.dxc6Do5Iakjs21cjCz2%2F%2Fg7Jfgiv2SmfLQ0iCyb%2FXSA; Path=/; HttpOnly
Vary: Accept-Encoding
Content-Length: 272
Connection: keep-alive

{"args":{},"data":"","files":{},"form":{"abcde":""},"headers":{"x-forwarded-proto":"https","host":"postman-echo.com","content-length":"5","content-type":"application/x-www-form-urlencoded","x-forwarded-port":"80"},"json":{"abcde":""},"url":"https://postman-echo.com/post"}
```
