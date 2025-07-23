TifFile = argv[1]

PythonCommand = "python3 /usr/local/freeswitch/conf/parseFax.py " .. TifFile
freeswitch.consoleLog("info", "onfax. running: '" .. PythonCommand .. "'\n");
os.execute(PythonCommand .. " > /tmp/parseFax.log &")

obSession = freeswitch.Session("sofia/192.168.188.161/local")
if obSession:ready() then
  freeswitch.consoleLog("info", "session is ready. is ready. Transmitting " .. TifFile .. " now.\n");
  obSession:execute("txFax", TifFile)
else   
  local obCause = obSession:hangupCause()
  freeswitch.consoleLog("info", "obSession:hangupCause() = " .. obCause )
end

