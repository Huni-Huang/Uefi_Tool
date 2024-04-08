Set Service = GetObject("winmgmts:{impersonationLevel=impersonate}!root/wmi")
Set enumSet = Service.InstancesOf("HuniWMIPortInterface")

For Each instance In enumSet
    result = instance.WMIMemory1(1)
    If result = 0 Then
        Wscript.Echo "Method executed successfully."
    Else
        Wscript.Echo "Method execution failed."
    End If
Next
