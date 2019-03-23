# c-httpping
httpping - A simple tool to ping HTTP servers and return a HTTP code

(c) Ben Sampson <https://github.com/billyrayvalentine>

# Usage
Ping a host forever
```
httpping www.nationalrail.co.uk
200 from HTTP://www.nationalrail.co.uk/ header size = 388 in 0.2 seconds seq=1
200 from HTTP://www.nationalrail.co.uk/ header size = 388 in 0.0 seconds seq=2
200 from HTTP://www.nationalrail.co.uk/ header size = 388 in 0.0 seconds seq=3
```

Ping just once
```
httpping www.nationalrail.co.uk -c1           
200 from HTTP://www.nationalrail.co.uk/ header size = 388 in 0.1 seconds seq=1
```

Ping a host forever (returns a 301 which isn't followed)
```
httpping www.mi5.gov.uk    
301 from HTTP://www.mi5.gov.uk/ header size = 373 in 0.1 seconds seq=1
301 from HTTP://www.mi5.gov.uk/ header size = 373 in 0.1 seconds seq=2
301 from HTTP://www.mi5.gov.uk/ header size = 373 in 0.1 seconds seq=3
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
httpping https://www.mi5.gov.uk/noobs.php -m 
404 https://www.mi5.gov.uk/noobs.php 606 0.1 1
404 https://www.mi5.gov.uk/noobs.php 606 0.2 2
404 https://www.mi5.gov.uk/noobs.php 606 0.2 3
```

## Args
```
Usage: httpping <url> [-cfmh?]
  -c, --number-pings=COUNT     number of times to ping
  -f, --follow-redirects       Follow HTTP 3XX redirects
  -m, --machine-readable       machine friendly output

Help options:
  -?, --help                   Show this help message
      --usage                  Display brief usage message

```

# Install
Needs libcurl-devel and popt
```
make
sudo make install
```
