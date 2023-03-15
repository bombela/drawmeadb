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
div.square {
	width: 100%;
	margin: 1em;
	padding: 1em;
}

h2 {
  font-weight: 800;
  top: -10px;
  text-decoration: underline;
}
</style>
