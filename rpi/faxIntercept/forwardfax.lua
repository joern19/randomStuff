TifFile = argv[1]

obSession = freeswitch.Session("sofia/192.168.188.161/local")
if obSession:ready() then
  freeswitch.consoleLog("info", "session is ready. is ready. Transmitting " .. TifFile .. " now.\n");

  obSession:setVariable("ignore_early_media", "true")
  obSession:setVariable("absolute_codec_string", "PCMU,PCMA")
  obSession:setVariable("fax_enable_t38", "true")
  obSession:setVariable("fax_verbose", "true")
  obSession:setVariable("fax_use_ecm", "true")
  obSession:setVariable("fax_enable_t38_request", "true")

  obSession:execute("txFax", TifFile)
else
  local obCause = obSession:hangupCause()
  freeswitch.consoleLog("info", "obSession:hangupCause() = " .. obCause )
end

