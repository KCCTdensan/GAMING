<script>
  import "bulma/css/bulma.css"

  const title = "GAMING"
  const description = "GAMING"
  const fdqn = "gaming.d3bu.net"
</script>

<svelte:head>
  <title>{title}</title>
  <meta name="description"  content="{description}" />
  <meta name="robots"       content="none" />

  <meta name="twitter:card"     content="summary" />
  <meta name="twitter:site"     content="@kcct_densan" />
  <meta name="twitter:creator"  content="@kcct_densan" />

  <meta property="og:type"        content="website" />
  <meta property="og:title"       content="{title}" />
  <meta property="og:description" content="{description}" />
  <meta property="og:site_name"   content="{title}" />
  <meta property="og:url"         content="https://{fdqn}/" />
  <meta property="og:image"       content="https://{fdqn}/icon.png" />
</svelte:head>

<div class="container">
  <slot />
</div>

<footer class="footer">
  <div class="content has-text-centered">
    <p>
      &copy; 2022-{new Date().getFullYear()}
      <a href="https://d3bu.net" target="_blank">KCCTdensan</a>
    </p>
  </div>
</footer>
