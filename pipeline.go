// measure the time to send a value through a pipeline of channels
//
// usage:
//         pipeline.go [-n numchan]
//
package main

import (
	"flag"
	"fmt"
	"time"
)

var n = flag.Int("n", 2048, "number of channels")

func main() {
	flag.Parse()
	ch0 := make(chan int)
	inCh := ch0
	var outCh chan int
	for i := 0; i < *n; i++ {
		outCh = make(chan int)
		go func(inCh <-chan int, outCh chan<- int) {
			outCh <- <-inCh
		}(inCh, outCh)
		inCh = outCh
	}
	start := time.Now()
	ch0 <- 0
	<-outCh
	elapsed := time.Since(start)
	fmt.Printf("nchan: %d elapsed: %s (%d ns/chan)\n", *n, elapsed, elapsed.Nanoseconds()/int64(*n))
}
