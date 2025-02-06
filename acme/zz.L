#!/usr/local/src/plan9port/bin/rc 

for(cmd in comp def fmt hov impls refs rn sig syms type assist ws ws+ ws-){
	> L^$cmd {
		echo '#!/usr/local/src/plan9port/bin/rc'
		echo exec L $cmd '$*'
	}
	chmod +x L^$cmd
}
