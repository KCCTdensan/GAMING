package gaming

import (
	"net/http"
	"strconv"

	"github.com/gin-gonic/gin"
	"golang.org/x/time/rate"
)

func init() { gin.SetMode(gin.ReleaseMode) }

type Server struct {
	router *gin.Engine
	addr   string
	status *Status
	cmd    *Cmd
}

var lim = rate.NewLimiter(2.0, 10)

func ratelimit(c *gin.Context) (limit bool) {
	if limit = !lim.Allow(); limit {
		c.String(http.StatusTooManyRequests, "Too Many Requests")
		c.Abort()
	}
	return
}

func NewServer(addr string, status *Status, cmd *Cmd) Server {
	router := gin.New()
	s := Server{router, addr, status, cmd}
	s.InitRoutes()
	return s
}

func (s *Server) InitRoutes() {
	s.router.GET("/angle", func(c *gin.Context) {
		c.String(http.StatusOK, strconv.Itoa(s.status.Angle))
	})
	s.router.GET("/type", func(c *gin.Context) {
		c.String(http.StatusOK, strconv.Itoa(s.status.Type))
	})
	s.router.GET("/delay", func(c *gin.Context) {
		c.String(http.StatusOK, strconv.Itoa(s.status.Delay))
	})

	s.router.POST("/suspend", func(c *gin.Context) {
		err := s.cmd.Suspend()
		if err != nil {
			c.String(http.StatusInternalServerError, "Internal Server Error")
			c.Abort()
			return
		}
	})
	s.router.POST("/reset", func(c *gin.Context) {
		err := s.cmd.Reset()
		if err != nil {
			c.String(http.StatusInternalServerError, "Internal Server Error")
			c.Abort()
			return
		}
		s.cmd.Refresh()
	})
	s.router.POST("/resend", func(c *gin.Context) {
		err := s.cmd.Resend()
		if err != nil {
			c.String(http.StatusInternalServerError, "Internal Server Error")
			c.Abort()
			return
		}
	})

	s.router.POST("/angle", func(c *gin.Context) {
		v := c.PostForm("val")
		n, err := strconv.Atoi(v)
		if err != nil {
			c.String(http.StatusBadRequest, "Bad Request")
			c.Abort()
			return
		}
		if s.status.Angle != n && !ratelimit(c) {
			err := s.cmd.Angle(n)
			if err != nil {
				c.String(http.StatusBadRequest, "Bad Request")
				c.Abort()
				return
			}
			s.cmd.Refresh()
		}
	})
	s.router.POST("/delay", func(c *gin.Context) {
		v := c.PostForm("val")
		n, err := strconv.Atoi(v)
		if err != nil {
			c.String(http.StatusBadRequest, "Bad Request")
			c.Abort()
			return
		}
		if s.status.Delay != n && !ratelimit(c) {
			err := s.cmd.Delay(n)
			if err != nil {
				c.String(http.StatusBadRequest, "Bad Request")
				c.Abort()
				return
			}
			s.cmd.Refresh()
		}
	})
	s.router.POST("/type", func(c *gin.Context) {
		v := c.PostForm("val")
		n, err := strconv.Atoi(v)
		if err != nil {
			c.String(http.StatusBadRequest, "Bad Request")
			c.Abort()
			return
		}
		if s.status.Type != n && !ratelimit(c) {
			err := s.cmd.Type(n)
			if err != nil {
				c.String(http.StatusBadRequest, "Bad Request")
				c.Abort()
				return
			}
			// s.cmd.Refresh()
		}
	})
}

func (s *Server) Run() {
	err := s.router.Run(s.addr)
	if err != nil {
		panic(err.Error())
	}
}
