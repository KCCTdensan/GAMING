export const get = async () => {
  const [angle, delay, type] = await Promise.all([
    fetch(`http://${process.env.API_HOST}/angle`).then(r => r.text()),
    fetch(`http://${process.env.API_HOST}/delay`).then(r => r.text()),
    fetch(`http://${process.env.API_HOST}/type`).then(r => r.text()),
  ])

  return {
    body: {
      type: +type,
      angle: +angle,
      delay: +delay,
    },
  }
}

export const post = async ({ request }) => {
  const val = await request.json()
  const [angle, delay, type] = [val.angle, val.delay, val.type]
    .map(v => new URLSearchParams({ val: `${v}` }))
  const res = await Promise.all([
    fetch(`http://${process.env.API_HOST}/angle`, {
      method: "POST",
      body: angle,
    }),
    fetch(`http://${process.env.API_HOST}/delay`, {
      method: "POST",
      body: delay,
    }),
    fetch(`http://${process.env.API_HOST}/type`, {
      method: "POST",
      body: type,
    }),
  ])
  const ratelimit = res.some(r => r.status === 429)
  return {
    status: ratelimit
      ? 429
      : res.some(r => !r.ok)
      ? 500
      : 200,
  }
}
