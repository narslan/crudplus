import { Component, inject } from '@angular/core';
import { FormBuilder } from '@angular/forms';

@Component({
  selector: 'app-add-album',
  standalone: true,
  imports: [],
  templateUrl: './add-album.component.html',
  styleUrl: './add-album.component.scss',
})
export class AddAlbumComponent {
  formBuilder: FormBuilder = inject(FormBuilder);

  albumeForm = this.formBuilder.group({
    name: [''],
    year: [''],
    picture: [],
  });
}
