export const get = async () => {
  const [angle, delay, type] = await Promise.all([
    fetch(`${process.env.API_BASE}/angle`).then(r => r.text()),
    fetch(`${process.env.API_BASE}/delay`).then(r => r.text()),
    fetch(`${process.env.API_BASE}/type`).then(r => r.text()),
  ])

  return {
    body: {
      type: +type,
      angle: +angle,
      delay: +delay,
    },
  }
}

export const post = async () => {
}
