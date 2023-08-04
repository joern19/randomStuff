package discord2voip.pjsua2;

import org.pjsip.pjsua2.AccountConfig;
import org.pjsip.pjsua2.AuthCredInfo;
import org.pjsip.pjsua2.Endpoint;
import org.pjsip.pjsua2.EpConfig;
import org.pjsip.pjsua2.TransportConfig;
import org.pjsip.pjsua2.pjsip_transport_type_e;

public class Pjsua2 {

  public static void start() {
    System.load("/home/pjproject-2.13.1/pjsip-apps/src/swig/java/output/libpjsua2.so");
    System.out.println("Library loaded");

    try {
      // Create endpoint
      Endpoint ep = new Endpoint();
      ep.libCreate();
      // Initialize endpoint
      EpConfig epConfig = new EpConfig();
      epConfig.getUaConfig().setThreadCnt(0);
      epConfig.getUaConfig().setMainThreadOnly(true);
      epConfig.getLogConfig().setLevel(5);
      epConfig.getLogConfig().setConsoleLevel(5);
      ep.libInit(epConfig);

      // Create SIP transport. Error handling sample is shown
      TransportConfig sipTpConfig = new TransportConfig();
      ep.transportCreate(pjsip_transport_type_e.PJSIP_TRANSPORT_UDP, sipTpConfig);
      // Start the library
      ep.libStart();

      Endpoint.instance().audDevManager().setNullDev();

      AccountConfig acfg = new AccountConfig();

      acfg.setIdUri("sip:pjsua2todiscord@192.168.1.1");
      acfg.getRegConfig().setRegistrarUri("sip:192.168.1.1");
      acfg.getRegConfig().setRegisterOnAdd(true);
      AuthCredInfo cred = new AuthCredInfo("digest", "*", "pjsua2todiscord", 0, "ThisPasswordHasToDifferToTheName");
      acfg.getSipConfig().getAuthCreds().add(cred);
      acfg.getPresConfig().setPublishEnabled(true);

      // Create the account
      MyAccount acc = new MyAccount(ep);
      acc.create(acfg);
      // Here we don't have anything else to do..

      while (true) {
        ep.libHandleEvents(10);
      }
    } catch (Exception e) {
      System.out.println(e);
      return;
    }
  }
}
