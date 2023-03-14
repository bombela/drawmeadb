<script setup lang="ts">
import { computed, onMounted } from 'vue'

const props = defineProps<{
  kind: string,
  solvedID?: string,
}>()

const placeholder_img = computed(() => "/placeholder_" + props.kind + ".png")
const img = computed(() => __PLAY_URL__ + props.solvedID + "/" + props.kind + ".png")
</script>

<template>
	<img v-if="solvedID === undefined" class="graph-render" :src=placeholder_img>
	<img v-else class="graph-render" :src=img>
</template>

<style scoped>
.graph-render_off {
	max-width: 100%;
}
</style>

