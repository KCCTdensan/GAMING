export const post = async ({ request }) => {
  const { pathname } = new URL(request.url)
  console.log(pathname)
  switch (true) {
    case pathname.startsWith("/suspend"):
    case pathname.startsWith("/resend"):
    case pathname.startsWith("/reset"):
      return fetch(`http://${process.env.API_HOST}${pathname}`, {
        method: "POST",
      })
    default:
      return { status: 404 }
  }
}
