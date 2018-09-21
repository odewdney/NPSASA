# NPSASA
NPS pluging to append TunnelGroup to the CalledStationId

This should allow creating NPS conditions depending on the Cisco ASA tunnel-group the user has connected to.

to install run:

regsvr32 npsasa.dll

When a user authenticates on a VPN connection the Called-Station-Id will be 1.2.3.4/Tunnel_Group where 1.2.3.4 is the ip address of the ASA from the original value of the Called-Station-Id and the Tunnel_Group is the value from the Vendor Attribute 3076:146

In windows IAS/NPS - add a Policy Condition on Called-Station-Id using a pattern match "/Tunnel_Group$".

So for example set a condition on User-Group AND the Called-Station-id to only allow members of a group to use that tunnel group
