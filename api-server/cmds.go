package gaming

import (
	"errors"
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
		c.ch <- []byte(s + "\n")
	}
}

func (c *Cmd) Refresh() error {
	c.exec("D")
	log.Print("Refresh")
	return nil
}
func (c *Cmd) Suspend() error {
	c.exec("S")
	log.Print("Suspend")
	return nil
}
func (c *Cmd) Reset() error {
	c.exec("R")
	log.Print("Reset")
	return nil
}
func (c *Cmd) Resend() error {
	c.Angle(c.status.Angle)
	c.Delay(c.status.Delay)
	c.Type(c.status.Type)
	log.Print("Resend")
	return nil
}

func (c *Cmd) Angle(n int) error {
	if !(0 <= n && n < 256) {
		return errors.New("out of range")
	}
	c.exec(fmt.Sprintf("a %d", n))
	c.status.Angle = n
	log.Printf("Angle = %d", n)
	return nil
}
func (c *Cmd) Delay(n int) error {
	if !(0 <= n && n < 256) {
		return errors.New("out of range")
	}
	c.exec(fmt.Sprintf("d %d", n))
	c.status.Delay = n
	log.Printf("Delay = %d", n)
	return nil
}
func (c *Cmd) Type(n int) error {
	if !(0 <= n && n < 256) {
		return errors.New("out of range")
	}
	c.exec(fmt.Sprintf("c %d", n))
	c.status.Type = n
	log.Printf("Type = %d", n)
	return nil
}
