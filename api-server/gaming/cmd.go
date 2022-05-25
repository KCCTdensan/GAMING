package gaming

import (
	"fmt"
	"log"
)

type Cmd struct {
	ch   chan []byte
	status *Status // for Type
}

func NewCmd(ch chan[]byte, status *Status) Cmd {
	return Cmd{ch, status}
}

func (c *Cmd) exec(s string) {
	c.ch <- []byte(s)
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
	log.Printf("Angle = %d", n)
}
func (c *Cmd) Delay(n int) {
	c.exec(fmt.Sprintf("d %d", n))
	log.Printf("Delay = %d", n)
}
func (c *Cmd) Type(n int) {
	c.exec(fmt.Sprintf("c %d", n))
	log.Printf("Type = %d", n)
	c.status.Type = n // temp
}
