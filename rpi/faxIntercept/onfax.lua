TifFile = argv[1]

PythonCommand = "python3 /usr/local/freeswitch/conf/parseFax.py " .. TifFile
freeswitch.consoleLog("info", "onfax. running: '" .. PythonCommand .. "'\n");
os.execute(PythonCommand .. " > /tmp/parseFax.log &")

