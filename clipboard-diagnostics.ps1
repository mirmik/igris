Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

if (-not ('ClipboardDiagnostics.NativeMethods' -as [type])) {
    Add-Type @'
using System;
using System.Runtime.InteropServices;

namespace ClipboardDiagnostics
{
    public static class NativeMethods
    {
        [DllImport("user32.dll", SetLastError = true)]
        public static extern IntPtr GetOpenClipboardWindow();

        [DllImport("user32.dll", SetLastError = true)]
        public static extern uint GetWindowThreadProcessId(
            IntPtr windowHandle,
            out uint processId
        );
    }
}
'@
}

$windowHandle = [ClipboardDiagnostics.NativeMethods]::GetOpenClipboardWindow()

if ($windowHandle -eq [IntPtr]::Zero) {
    Write-Output 'Окно, удерживающее буфер обмена, не найдено.'
    Write-Output 'Возможно, блокировка уже исчезла или приложение открыло буфер без привязки к окну.'
    exit 2
}

[uint32]$processId = 0
[void][ClipboardDiagnostics.NativeMethods]::GetWindowThreadProcessId(
    $windowHandle,
    [ref]$processId
)

try {
    $process = Get-Process -Id $processId
} catch {
    Write-Error "Окно найдено, но получить процесс с ID $processId не удалось: $($_.Exception.Message)"
    exit 1
}

$processPath = try {
    $process.Path
} catch {
    '<недоступно>'
}

[pscustomobject]@{
    Id              = $process.Id
    ProcessName     = $process.ProcessName
    Path            = $processPath
    MainWindowTitle = $process.MainWindowTitle
    WindowHandle    = ('0x{0:X}' -f $windowHandle.ToInt64())
}
