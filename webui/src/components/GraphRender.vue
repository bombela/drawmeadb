<script setup lang="ts">
import { computed, onMounted } from 'vue'

const props = defineProps<{
  kind: string,
  solvedID?: string,
}>()

const placeholder_img = computed(() => {
	return `${import.meta.env.BASE_URL}/placeholder_${props.kind}.png`;
})
const img = computed(() => `${__PLAY_URL__}/${props.solvedID}/${props.kind}.png`)
</script>

<template>
	<div class="imgbox">
		<img v-if="solvedID === undefined" class="graph-render" :src=placeholder_img>
		<img v-else class="graph-render" :src=img>
	</div>
</template>

<style scoped>
div.imgbox {
	border: 1px solid var(--color-border);
	border-radius: 4px;
	height: 100%;
}
img.graph-render {
	width: 100%;
	height: 100%;
	object-fit: contain;
}
</style>

