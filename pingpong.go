// measure the time to pingpong a value on a channel
//
// usage:
//         pingpong.go [-n numsend]
//
package main

import (
	"flag"
	"fmt"
	"time"
)

var n = flag.Int("n", 1024, "number of sends")

func main() {
	flag.Parse()
	ch0 := make(chan int)
	ch1 := make(chan int)
	go func() {
		for range ch0 {
			ch1 <- 0
		}
	}()

	start := time.Now()
	for i := 0; i < *n; i++ {
		ch0 <- 0
		<-ch1
	}
	elapsed := time.Since(start)
	fmt.Printf("nsend: %d elapsed: %s (%d ns/send)\n", *n, elapsed, elapsed.Nanoseconds()/int64(*n))
}
