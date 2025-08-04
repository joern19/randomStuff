TifFile = session:getVariable("fax_file")

PythonCommand = "python3 /usr/local/freeswitch/conf/parseFax.py " .. TifFile
freeswitch.consoleLog("info", "onfax. running: '" .. PythonCommand .. "'\n");
os.execute(PythonCommand .. " > /tmp/parseFax.log &")
pcall(function () os.execute("kill -USR1 2") end)
