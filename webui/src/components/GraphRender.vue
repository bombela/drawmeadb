<script setup lang="ts">
defineProps<{
  placeholder_img: string,
}>()
</script>

<template>
	<img class="graph-render" v-bind:src=placeholder_img>
</template>

<style scoped>
.graph-render {
	width: 100%;
}
</style>

