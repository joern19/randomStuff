# Compiling module and binary

The reddit post was really helpful, to build wireguard from source in a chroot that was created using the github repo

https://github.com/SynologyOpenSource/pkgscripts-ng

https://www.reddit.com/r/synology/comments/a2erre/guide_intermediate_how_to_install_wireguard_vpn/

# Setup

ip link add wg0 type wireguard
ip addr add fd00:1132:1f28:e57a::2/64 dev wg0
wg set wg0 private-key ~/privatekey
ip link set wg0 up
wg set wg0 peer <gateway public key> allowed-ips fd00:1132:1f28:e57a::1/128 endpoint gateway.hirschfeld.tech:51820

TODO: setup firewall rules on the synology: Only allow traffic from wireguard subnet.
