<script context="module">
  export const load = ({ fetch }) => ({ props: { fetch } })
</script>

<script>
  export let fetch

  let type = 0
  let angle = 0
  let delay = 0
  $: guruguru = false

  const sleep = s => new Promise(r => setTimeout(r, s * 1000))

  const get = async () => ({ type, angle, delay } = await fetch("/api").then(r => r.json()))

  const update = async () => {
    guruguru = true
    const res = await fetch("/api", {
      method: "POST",
      body: JSON.stringify({ type, angle, delay }),
    })
    if (res.status === 429) {
      await sleep(1)
      return update()
    }
    get()
    guruguru = false
  }
</script>

<h1>GAMING</h1>

{#if guruguru}
  guruguru...
{/if}

{#await get()}
  <p>loading...</p>
{:then}
  <h2>Type</h2>
  <label>
    <input type="number" bind:value={type} on:change={update}>
  </label>

  <h2>Angle</h2>
  <label>
    <input type="number" bind:value={angle} on:change={update}>
    <input type="range" bind:value={angle} on:change={update}>
  </label>

  <h2>Delay</h2>
  <label>
    <input type="number" bind:value={delay} on:change={update}>
    <input type="range" bind:value={delay} on:change={update}>
  </label>
{/await}
