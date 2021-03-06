# Detect if a Folder is created

### Configuration

`requestMesthod` may be `GET`, `PUT`, `DELETE`, `HEAD`, `POST`, `PATCH` or `OPTIONS` \
`url`: `{emailID}` will be replaced with the name of the first folder. `{dateID}` will be replaced with the name of the second folder. Example: http://localhost:8000/{emailID}/{dateID} \
`rootFilePath`: The path to look for folder creations. The request will be executed if a folder is created 2 layers deep. For example using `/home/test`, a request will be executed if the folder `/home/test/myFirstFolder/mySecondFolder` is created. Then the url would result in `http://localhost:8000/myFirstFolder/mySecondFolder`  

### Installation

After configuring, you can install the script with `sudo ./install.sh`. It will copy the files to the right folders and starts the service. Any changes after the installation in folderDetectConfig.yaml will have no effect on the installation. You can delete this folder now.

### Modify the configuration

The configuration is located in `/usr/local/lib/folderDetect/folderDetectConfig.yaml`. after modification you need to restart the service.
You can restart the service using `sudo systemctl restart folderDetect` 

### Uninstall

`sudo ./uninstall.sh` will delete all files created, but will not remove the library installed with pip because you may need them.
The Librarys installed with pip in the Installation process are: `pyyaml`, `watchdog` and `requests` 
