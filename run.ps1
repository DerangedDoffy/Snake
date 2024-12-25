$path = split-path -parent $MyInvocation.MyCommand.Definition
$input = "$($path)\snake.c"
$output = "$($path)\snake.exe"

gcc $input -o $output 2>&1
Invoke-Expression $output