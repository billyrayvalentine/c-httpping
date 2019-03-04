# c-httpping
httpping
A simple tool to ping HTTP servers and return a HTTP code
(c) Ben Sampson <https://github.com/billyrayvalentine>

# Usage
Ping a host forever
```
httpping www.nationalrail.co.uk
200 from HTTP://www.nationalrail.co.uk/ header size = 388 in 0.2 seconds seq=1
200 from HTTP://www.nationalrail.co.uk/ header size = 388 in 0.0 seconds seq=2
200 from HTTP://www.nationalrail.co.uk/ header size = 388 in 0.0 seconds seq=3
```

Same as above but follow 3XX redirects
```
httpping www.mi5.gov.uk -f
200 from https://www.mi5.gov.uk/ header size = 1158 in 0.3 seconds seq=1
200 from https://www.mi5.gov.uk/ header size = 1158 in 0.2 seconds seq=2
200 from https://www.mi5.gov.uk/ header size = 1158 in 0.2 seconds seq=3
```

Print in machine readable format
```
httpping www.mi5.gov.uk -f -m
200 https://www.mi5.gov.uk/ 1158 0.3 1
200 https://www.mi5.gov.uk/ 1158 0.2 2
200 https://www.mi5.gov.uk/ 1158 0.4 3
```

# Install
Needs libcurl-devel
```
make
sudo make install
```
