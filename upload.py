Import("env")

def on_upload(source,target,env):
    print(source, target)
    firmware_path = str(source[0])
    #env.Execute("$UPLOADER -e -w -p /dev/ttyACM0 bootloader.bin")
    env.Execute("$UPLOADER -w  --boot=1 -R .pio/build/due/firmware.bin")
    env.Execute("stty -F /dev/ttyACM0 115200")
env.Replace(UPLOADCMD=on_upload)    
