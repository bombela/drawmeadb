<script setup lang="ts">
defineProps<{
  title: string,
}>()
</script>

<template>
	<div class="square">
		<p>{{ title }}</p>
		<div>
			<slot></slot>
		</div>
	</div>
</template>

<style scoped>
.square {
	border: solid 1px orange;
	border-radius: 4px;
	width: 100%;
	margin: 10px;
	padding: 10px;
}

h1 {
  font-weight: 500;
  font-size: 2.6rem;
  top: -10px;
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
