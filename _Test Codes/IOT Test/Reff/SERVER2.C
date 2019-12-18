AT+CWMODE=3


OK
AT+CIPMUX=1


OK
AT+CWSAP="MY_Wifi_SubStation","1234567890",5,3


OK
AT+CIPSERVER=1,80


OK
0,CONNECT

+IPD,0,15:hello world....1,CONNECT
2,CONNECT

+IPD,1,366:GET / HTTP/1.1
Host: 192.168.4.1
Connection: keep-alive
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
User-Agent: Mozilla/5.0 (Linux; Android 4.1.2; GT-S7262 Build/JZO54K) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/28.0.1500.94 Mobile Safari/537.36
Accept-Encoding: gzip,deflate,sdch
Accept-Language: en-GB,en-US;q=0.8,en;q=0.6

2,CLOSED
0,CLOSED
AT+CIPSEND=1,21


OK
> 

busy s...

Recv 21 bytes

SEND OK
1,CLOSED
0,CONNECT
1,CONNECT
2,CONNECT

+IPD,1,411:GET / HTTP/1.1
Host: 192.168.4.1
Connection: keep-alive
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (Linux; Android 5.1; MotoE2 Build/LPC23.13-56) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/49.0.2623.105 Mobile Safari/537.36
Accept-Encoding: gzip, deflate, sdch
Accept-Language: en-IN,en-US;q=0.8,en;q=0.6

1,CLOSED

+IPD,2,411:GET / HTTP/1.1
Host: 192.168.4.1
Connection: keep-alive
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (Linux; Android 5.1; MotoE2 Build/LPC23.13-56) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/49.0.2623.105 Mobile Safari/537.36
Accept-Encoding: gzip, deflate, sdch
Accept-Language: en-IN,en-US;q=0.8,en;q=0.6

AT+CIPSEND=0,21


OK
> 

busy s...

Recv 21 bytes

SEND OK
AT+CIPSEND=1,21

link is not valid

ERROR
AT+CIPCLOSE=0

0,CLOSED

OK
AT+CIPSEND=1,21

link is not valid

ERROR
2,CLOSED
0,CONNECT

+IPD,0,411:GET / HTTP/1.1
Host: 192.168.4.1
Connection: keep-alive
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (Linux; Android 5.1; MotoE2 Build/LPC23.13-56) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/49.0.2623.105 Mobile Safari/537.36
Accept-Encoding: gzip, deflate, sdch
Accept-Language: en-IN,en-US;q=0.8,en;q=0.6

AT+CIPSEND=0,21


OK
> 

busy s...

Recv 21 bytes

SEND OK
1,CONNECT

+IPD,1,15:<h1>HELLO WORLD2,CONNECT

+IPD,2,351:GET /favicon.ico HTTP/1.1
Host: 192.168.4.1
Connection: keep-alive
User-Agent: Mozilla/5.0 (Linux; Android 5.1; MotoE2 Build/LPC23.13-56) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/49.0.2623.105 Mobile Safari/537.36
Accept: */*
Referer: http://192.168.4.1/
Accept-Encoding: gzip, deflate, sdch
Accept-Language: en-IN,en-US;q=0.8,en;q=0.6

3,CONNECT

+IPD,3,437:GET / HTTP/1.1
Host: 192.168.4.1
Connection: keep-alive
Cache-Control: max-age=0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (Linux; Android 5.1; MotoE2 Build/LPC23.13-56) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/49.0.2623.105 Mobile Safari/537.36
Accept-Encoding: gzip, deflate, sdch
Accept-Language: en-IN,en-US;q=0.8,en;q=0.6

3,CLOSED
3,CONNECT
4,CONNECT
3,CLOSED
4,CLOSED
3,CONNECT

+IPD,3,437:GET / HTTP/1.1
Host: 192.168.4.1
Connection: keep-alive
Cache-Control: max-age=0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (Linux; Android 5.1; MotoE2 Build/LPC23.13-56) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/49.0.2623.105 Mobile Safari/537.36
Accept-Encoding: gzip, deflate, sdch
Accept-Language: en-IN,en-US;q=0.8,en;q=0.6

0,CLOSED
1,CLOSED
2,CLOSED
3,CLOSED
