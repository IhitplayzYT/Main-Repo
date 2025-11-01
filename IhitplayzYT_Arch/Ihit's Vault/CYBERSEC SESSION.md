RTO(Recovery Time Objective)  : Max time service can be down before restoring.

RPO(Recovery Point objective) : Max data that can be lost due to downtime.

SYMMETRIC - Same Key for Encrypt and decrypt.eg: DES(BROKEN/OLD),AES(CURR),3DES(BROKEN/OLD)

DES - 64 BYTES 56 BYTES INFO AND 8 BYTES PARITY.

ASYMMETRIC - Diff key for decrypt and encrypt.eg: RSA,ECC

SENDING ENCRYPTED DATA:
- Encrypted by receivers public key and this decrypted by receivers priv key.
VERIFYING/CERTIFYING SENDER:
- Sender encrypts message with their priv key and this decrypted by receiver using available pub key of sender.

Priv & Pub keys are mathematically specially derived to make sure same mssg decrypted by both.

RAID- redundant array of inexpensive disks
- Raid-0(Striping) : Data filled by moving between disks,fast R/W.
- Raid-1(Mirroring) : Clone of one disk
- Raid-4(Parity) : Parity byte for recovery
- Raid-5(Stripping + parity) : Parity bits distributed between the disk in alternating fashion.
- Raid-10(Combination of 0 and 1) : Data stripped between two or more mirrored drives.

SSH(Secure shell)

DOS(Denial of Service) & DDOS(Distributed Denial of Service)

TCP is reliable,slower,connection-oriented,bulky and used in web,ssh,ftp,etc.

UDP is connectionless,faster,lightweigth,no error detection, and less reliable used in videos,gaming,etc.

ttl - decides the max no of hops that a packet can do before death.

TCP - Transmitted in a 3 way handshake:
- SYN - sending to server
- SYN+AWK - Receiver sends to sender
- AWK - Sender send data to reciever

Headers transmitted in network byte order

IP - 4 byte field(32 bytes)

Mac - 6 byte field(48 bytes)

Hashcat:
-a 0 file ->use words from this
-m x ->type of hash(0-md5,1400-sha256)

eg : hashcat -m 0 -a 0 hash.txt rockyou.txt --show

John:
john --format=raw-md5 --wordlist=/path/to/rockyou.txt hash.txt --show

scalpel - file carving and recovery (scalpel target -o output)
lynis - Security auditing
exiftool - exif data 
FTK imager - To create a disk image

Write Blocker - Only allows read to devices.

(pdcdc) preventive detective corrective detterant compensating control(alternateive way to control the issue)

CIA TRIAD:
Confidentiality : Unauthorised access is restricted.
Integrity : Unauthorized writing is restricted.
Accessibility : Easy access to resource when needed

To grant access: 
	NEED TO KNOW
	AUTHORIZATION AND AUTHENTICATION
	
PPTL(PERSON PROCESS TECH LOCATION) - To analyse secure systems by looking at all aspects of the model.

DNS(Domain Name System) : Converts human readable names to ip addresses.

CLOUD OUTAGES- AWS AND CLOUDFLARE

BACKUP OF DATA- HOW IS IT PRESERVED

IRP(INCIDENCE REPONSE PLAN) - Structured set of procedures for responding,and controling to incidents.

TPM(Trustred Platform Module): A hardware chip that designed to provide hardware based security .

FM200 FIRE EXTINGUISHER

BMS - BUILDING MANAGEMENT SYSTEM

OSI model
	**Physical** : Transmission of bits(wires,fibres,radio,hubs,etc)
	**Data** : Mac addresses,framing and error detection.
	**Network** : Ip and routing
	**Transport** : Data delivery TCP,UDP,ports
	**Session** : Tokens,RPC,NetBios
	**Presentation** : Format of data and encryption,decryption eg png,docs,etc
	**Application** : User interface and networking service.eg- HTTP,HTTPS,FTP,SMTP,etc


DHCP(Dynamic host config protocol) : Automatic local IP for devices in the LAN.

HTTPS: transfers data with encryption like TLS/SSL.

Default Port
HTTP 80
HTTPS 443
SSH 22
SMTP 25
DNS 53

TLS(Transport layer security)
SSL(Secure sockets layer)

Some virus have similar hashes and can be identified by their respective hashes

IDS (Intrusion Detection System) and IPS(Intrusion Prevention System) : Detect identify,monitor and defelect/block malicious activity

Antivirus - Isolates and deletes the malicious software.

TCP HEADER:
FLAGs:
	CWR - Congestion control flag
	ECN
	PSH - Buffered data to be pushed to application layer
	URG - Urgent ptr field required
	ACK - acknowledgement flag
	RST - Reset connection
	SYN - sync seq no
	FIN - Last packet end connection

