package gaming

import (
	"fmt"
	"log"
)

type Cmd struct {
	ch     chan []byte
	status *Status
	demo   bool
}

func NewCmd(ch chan []byte, status *Status, demo bool) Cmd {
	return Cmd{ch, status, demo}
}

func (c *Cmd) exec(s string) {
	if !c.demo {
		c.ch <- []byte(s)
	}
}

func (c *Cmd) Refresh() {
	c.exec("D")
	log.Print("Refresh")
}
func (c *Cmd) Suspend() {
	c.exec("S")
	log.Print("Suspend")
}
func (c *Cmd) Reset() {
	c.exec("R")
	log.Print("Reset")
}

func (c *Cmd) Angle(n int) {
	c.exec(fmt.Sprintf("a %d", n))
	if c.demo {
		c.status.Angle = n
	}
	log.Printf("Angle = %d", n)
}
func (c *Cmd) Delay(n int) {
	c.exec(fmt.Sprintf("d %d", n))
	if c.demo {
		c.status.Delay = n
	}
	log.Printf("Delay = %d", n)
}
func (c *Cmd) Type(n int) {
	c.exec(fmt.Sprintf("c %d", n))
	if c.demo || true { // temp
		c.status.Type = n
	}
	log.Printf("Type = %d", n)
}
