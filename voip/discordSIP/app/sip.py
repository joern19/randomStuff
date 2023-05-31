import pjsua2 as pj
from sip_account import Account

ep = pj.Endpoint()
ep.libCreate()

ep_cfg = pj.EpConfig()
ep_cfg.uaConfig.threadCnt = 0
ep_cfg.uaConfig.mainThreadOnly = True
ep_cfg.logConfig.level = 5
ep_cfg.logConfig.consoleLevel = 5
#ep_cfg.medConfig.sndAutoCloseTime = -1

ep.libInit(ep_cfg)

tc_udp = pj.TransportConfig()
#tc_upd.portRange = 4
tc_udp.port = 56447
ep.transportCreate(pj.PJSIP_TRANSPORT_UDP, tc_udp)
#tc_tcp = pj.TransportConfig()
#tc_tcp.randomizePort = True
#ep.transportCreate(pj.PJSIP_TRANSPORT_TCP, tc_tcp)

ep.libStart()

pj.Endpoint.instance().audDevManager().setNullDev()

accountConfig: pj.AccountConfig = pj.AccountConfig()

accountConfig.idUri = "sip:pjsua2todiscord@192.168.1.1"
accountConfig.regConfig.registrarUri = "sip:192.168.1.1"
accountConfig.regConfig.registerOnAdd = True

accountConfig.sipConfig.authCreds.append(pj.AuthCredInfo("digest", "*", "pjsua2todiscord", 0, "ThisPasswordHasToDifferToTheName"))
#accountConfig.sipConfig.proxies.append("sip:192.168.1.1:5060;transport=udp")
accountConfig.presConfig.publishEnabled = True

#accountConfig.mediaConfig.useLoopMedTp = True
#accountConfig.mediaConfig.transportConfig.portRange = 1

#accountConfig.natConfig.sipStunUse = False
#accountConfig.natConfig.turnEnabled = False
#accountConfig.natConfig.sipUpnpUse = False
#accountConfig.natConfig.mediaUpnpUse = False


acc = Account(ep)
acc.create(accountConfig)

while True:
    ep.libHandleEvents(10)

#call.delete()
#ep.libDestroy()
#ep.delete()
