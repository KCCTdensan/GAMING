<script context="module">
  export const load = ({ fetch }) => ({ props: { fetch } })
</script>

<script>
  export let fetch

  let type = 0
  let angle = 0
  let delay = 0

  const init = fetch("/api").then(async r => { if(r.ok) {
    ;({ type, angle, delay } = await r.json())
  }})

  const update = async () => {
  }
</script>

<h1>GAMING</h1>

{#await init}
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
