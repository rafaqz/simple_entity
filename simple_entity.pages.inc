<?php

/**
 * Simple Entity view callback.
 */
function simple_entity_view($simple_entity) {
  drupal_set_title(entity_label('simple_entity', $simple_entity));
  return entity_view('simple_entity', array(entity_id('simple_entity', $simple_entity) => $simple_entity), 'full');
}
