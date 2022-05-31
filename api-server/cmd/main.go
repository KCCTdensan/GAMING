package main

import (
	"flag"
	"log"

	"api-server"
)

var (
	Addr = flag.String("addr", ":8080", "listen address")
	Dev  = flag.String("dev", "/dev/ttyUSB0", "serial device")
	Baud = flag.Int("baud", 115200, "serial baud rate")
	Demo = flag.Bool("demo", false, "demo mode (disables serial)")
)

func init() { flag.Parse() }

func main() {
	var status gaming.Status
	chin := make(chan []byte)
	chout := make(chan []byte)
	cmd := gaming.NewCmd(chout, &status, *Demo)

	// serial
	if *Demo {
		log.Print("running in demo mode...")
	} else {
		s := gaming.NewSerial(*Dev, *Baud)
		go s.Print(chout)
		go s.Listen(chin)
		go s.Parse(&status, chin)
		cmd.Refresh()
	}

	// http
	r := gaming.NewServer(*Addr, &status, &cmd)
	r.Run()

	log.Print("shutting down...")
}
