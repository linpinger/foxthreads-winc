	gosub, initVar

;	gosub, single ; 5672
	gosub, multi  ; 1328
return

single:
	sTime := A_TickCount

	loop, parse, lists, `n
		runwait, wget -O c:\m%A_index%.html "%A_LoopField%"

	eTime := A_TickCount - sTime
	TrayTip, ºÄÊ±:, %eTime% ms
return

multi: ; 2797
	EnvSet, FoxTaskCount, %listCount%
	loop, parse, lists, `n
		EnvSet, FoxTask%A_index%, wget -O c:\m%A_index%.html "%A_LoopField%"

	sTime := A_TickCount
	runwait, FoxThreads.exe
	eTime := A_TickCount - sTime
	TrayTip, ºÄÊ±:, %eTime% ms
return

initVar:
lists =
(join`n
http://www.autohotkey.com
http://www.ahkscript.org
http://linpinger.github.io
)
	listCount := 18
return
