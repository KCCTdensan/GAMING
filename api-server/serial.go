package gaming

import (
	"strconv"
	"strings"

	"github.com/tarm/serial"
)

type Serial struct {
	serial *serial.Port
}

func NewSerial(dev string, baud int) Serial {
	c := &serial.Config{
		Name: dev,
		Baud: baud,
	}
	s, err := serial.OpenPort(c)
	if err != nil {
		panic(err.Error())
	}
	return Serial{s}
}

func (s *Serial) Print(ch chan []byte) {
	for b := range ch {
		s.serial.Write(b)
	}
}

func (s *Serial) Listen(ch chan []byte) {
	b := make([]byte, 128)
	for {
		n, err := s.serial.Read(b)
		if err != nil {
			panic(err.Error())
		}
		ch <- b[:n]
	}
}

func (Serial) Parse(status *Status, ch chan []byte) {
	for b := range ch {
		s := string(b)
		if a := strings.Split(s[:len(s)-2], ","); len(a) == 2 {
			v0, err := strconv.Atoi(a[0])
			if err != nil {
				continue
			}
			v1, err := strconv.Atoi(a[1])
			if err != nil {
				continue
			}
			status.Angle = v0
			status.Delay = v1
		}
	}
}
