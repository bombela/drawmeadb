<script setup lang="ts">
defineProps<{
  title: string,
}>()
</script>

<template>
	<div class="square">
		<h2>{{ title }}</h2>
		<div class="content">
			<slot></slot>
		</div>
	</div>
</template>

<style scoped>
.square {
	width: 100%;
	margin: 10px;
	padding: 2.5em;
}

h2 {
  font-weight: 800;
  top: -10px;
  text-decoration: underline;
}

h3 {
  font-size: 1.2rem;
}

.greetings h1,
.greetings h3 {
  text-align: center;
}

@media (min-width: 1024px) {
  .greetings h1,
  .greetings h3 {
    text-align: left;
  }
}
</style>
