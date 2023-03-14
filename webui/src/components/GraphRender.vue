<script setup lang="ts">
import { computed, onMounted } from 'vue'

const props = defineProps<{
  kind: string,
  solvedID?: string,
}>()

const placeholder_img = computed(() => "placeholder_" + props.kind + ".png")
const img = computed(() => __PLAY_URL__ + props.solvedID + "/" + props.kind + ".png")
</script>

<template>
	<template v-if="solvedID === undefined">
		<img class="graph-render" :src=placeholder_img>
	</template>
	<template v-else>
		<img class="graph-render" :src=img>
	</template>
</template>

<style scoped>
.graph-render_off {
	max-width: 100%;
}
</style>

