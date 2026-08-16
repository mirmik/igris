Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

if (-not ('ClipboardDiagnosticsNativeMethods' -as [type])) {
    Add-Type -TypeDefinition @'
using System;
using System.Runtime.InteropServices;

public static class ClipboardDiagnosticsNativeMethods
{
    [DllImport("user32.dll", SetLastError = true)]
    public static extern IntPtr GetOpenClipboardWindow();

    [DllImport("user32.dll", SetLastError = true)]
    public static extern uint GetWindowThreadProcessId(
        IntPtr windowHandle,
        out uint processId
    );
}
'@
}

$windowHandle = [ClipboardDiagnosticsNativeMethods]::GetOpenClipboardWindow()

if ($windowHandle -eq [IntPtr]::Zero) {
    Write-Output 'No window currently holding the clipboard was found.'
    Write-Output 'The lock may have disappeared, or the application may have opened the clipboard without a window handle.'
    exit 2
}

[uint32]$processId = 0
[void][ClipboardDiagnosticsNativeMethods]::GetWindowThreadProcessId(
    $windowHandle,
    [ref]$processId
)

try {
    $process = Get-Process -Id $processId
} catch {
    Write-Error "A window was found, but process ID $processId could not be inspected: $($_.Exception.Message)"
    exit 1
}

$processPath = try {
    $process.Path
} catch {
    '<unavailable>'
}

[pscustomobject]@{
    Id              = $process.Id
    ProcessName     = $process.ProcessName
    Path            = $processPath
    MainWindowTitle = $process.MainWindowTitle
    WindowHandle    = ('0x{0:X}' -f $windowHandle.ToInt64())
}
